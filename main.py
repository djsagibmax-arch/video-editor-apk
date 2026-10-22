import os
import subprocess

# ==========================================
# স্পিড ১.১x, কালার ও অডিও ক্লিনিং সহ ব্যাচ ভিডিও এডিটর
# ==========================================

INPUT_DIR = "/content/input_videos"
OUTPUT_DIR = "/content/output_videos"

if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

print("ভিডিও প্রসেসিং টুল প্রস্তুত হচ্ছে...")

video_files = [f for f in os.listdir(INPUT_DIR) if f.endswith(('.mp4', '.mkv', '.mov', '.avi'))]

if not video_files:
    print("❌ ইনপুট ফোল্ডারে কোনো ভিডিও পাওয়া যায়নি!")
else:
    print(f"📁 মোট {len(video_files)}টি ভিডিও পাওয়া গেছে। কাজ শুরু হচ্ছে...")

    for index, filename in enumerate(video_files[:10], start=1):
        input_path = os.path.join(INPUT_DIR, filename)
        output_filename = f"speed_edited_{filename}"
        output_path = os.path.join(OUTPUT_DIR, output_filename)

        print(f"\n⏳ [{index}/10] প্রসেস ও স্পিড বাড়ানো হচ্ছে: {filename} ...")

        # FFmpeg কমান্ড:
        # ১. -vf 'eq=brightness=0.08,setpts=1/1.1*PTS' -> ব্রাইটনেস বাড়ানো এবং ভিডিওর স্পিড ১.১ গুণ করা
        # ২. -af 'afftdn=nf=-25,atempo=1.1' -> ব্যাকগ্রাউন্ড নয়েজ রিমুভ করা এবং অডিওর স্পিড ১.১ গুণ করা
        
        ffmpeg_command = [
            'ffmpeg', '-y',
            '-i', input_path,
            '-vf', 'eq=brightness=0.08,setpts=1/1.1*PTS',  # ব্রাইটনেস ও ১.১x ভিডিও স্পিড
            '-af', 'afftdn=nf=-25,atempo=1.1',             # নয়েজ রিমুভ ও ১.১x অডিও স্পিড
            '-c:v', 'libx264',
            '-crf', '23',
            '-c:a', 'aac',
            '-b:a', '128k',
            '-ar', '44100',
            '-ac', '2',
            output_path
        ]

        try:
            subprocess.run(ffmpeg_command, check=True)
            print(f"✅ সফলভাবে সম্পন্ন! সেভ হয়েছে: {output_filename}")
        except subprocess.CalledProcessError as e:
            print(f"❌ ফাইল {filename} এডিট করার সময় সমস্যা হয়েছে:", e)

print("\n🎉 আলহামদুলিল্লাহ! আপনার সবগুলো ভিডিওর স্পিড ও এডিটিং সফলভাবে শেষ হয়েছে।")
